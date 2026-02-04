/**
 Reports detailed information from an attempted run of the SVG Parser
 */
#import <Foundation/Foundation.h>

@class SVGSVGElement, SVGDocument;
#import "SVGSVGElement.h"
#import "SVGDocument.h"

@protocol SVGKParserExtension;
#import "SVGKParserExtension.h"

@class SVGElement;

/*! Block type for custom identifier resolution
 * 
 * This block allows you to customize how identifiers are generated for SVGElement instances
 * during parsing. The block receives the SVGElement being processed and should return the
 * identifier string to use, or nil to fall back to the default behavior (using the 'id' attribute).
 * 
 * This is useful when:
 * - SVG nodes don't have 'id' attributes
 * - SVG nodes have duplicate 'id' values
 * - You want to use business-specific identifiers for quick CALayer lookup via dictionaryOfLayers
 * 
 * Example usage:
 *   parser.identifierResolver = ^NSString *(SVGElement *element) {
 *       // Use a custom attribute as identifier
 *       NSString *customId = [element getAttribute:@"data-id"];
 *       if (customId.length > 0) {
 *           return customId;
 *       }
 *       // Fall back to original id attribute
 *       return [element getAttribute:@"id"];
 *   };
 * 
 * @param element The SVGElement being processed
 * @return The custom identifier to use, or nil to use the default 'id' attribute
 */
typedef NSString * _Nullable (^SVGKParserIdentifierResolver)(SVGElement * _Nonnull element);

@interface SVGKParseResult : NSObject

@property(nonatomic, strong) NSMutableArray* _Nullable warnings;
@property(nonatomic, strong) NSMutableArray* _Nullable errorsRecoverable;
@property(nonatomic, strong) NSMutableArray* _Nullable errorsFatal;
@property(nonatomic) BOOL libXMLFailed;
/** 0.0 = no parsing done yet, 0.x = partially parsed, 1.0 = parse complete (no fatal errors) */
@property(nonatomic) double parseProgressFractionApproximate;

@property(nonatomic,strong) SVGSVGElement* _Nullable rootOfSVGTree;/**< both are needed, see spec */
@property(nonatomic,strong) SVGDocument* _Nullable parsedDocument; /**< both are needed, see spec */

@property(nonatomic,strong) NSMutableDictionary* _Nullable namespacesEncountered; /**< maps "prefix" to "uri" */

/*! Optional block to customize identifier generation for SVGElement instances during parsing
 * 
 * This is set by SVGKParser and used by SVGElement during post-processing.
 * If nil, the default behavior (using the 'id' attribute) is used.
 * 
 * @see SVGKParserIdentifierResolver for more details
 */
@property(nonatomic,copy,nullable) SVGKParserIdentifierResolver identifierResolver;

-(void) addSourceError:(NSError* _Nonnull) fatalError;
-(void) addParseWarning:(NSError* _Nonnull) warning;
-(void) addParseErrorRecoverable:(NSError* _Nonnull) recoverableError;
-(void) addParseErrorFatal:(NSError* _Nonnull) fatalError;
-(void) addSAXError:(NSError* _Nonnull) saxError;

#if ENABLE_PARSER_EXTENSIONS_CUSTOM_DATA
/*! Each SVGKParserExtension can optionally save extra data here */
@property(nonatomic,retain) NSMutableDictionary* _Nullable extensionsData;

-(NSMutableDictionary* _Nullable) dictionaryForParserExtension:(NSObject<SVGKParserExtension>* _Nonnull) extension;
#endif

@end
